// Copyright (c) 2010-2020, Lawrence Livermore National Security, LLC. Produced
// at the Lawrence Livermore National Laboratory. All Rights reserved. See files
// LICENSE and NOTICE for details. LLNL-CODE-806117.
//
// This file is part of the MFEM library. For more information and source code
// availability visit https://mfem.org.
//
// MFEM is free software; you can redistribute it and/or modify it under the
// terms of the BSD-3 license. We welcome feedback and contributions, see file
// CONTRIBUTING.md for details.

#include "../general/forall.hpp"
#include "nonlininteg.hpp"
#include "libceed/nlconvection.hpp"

using namespace std;

namespace mfem
{
void VectorConvectionNLFIntegrator::AssembleMF(const FiniteElementSpace &fes)
{
   MFEM_ASSERT(fes.GetOrdering() == Ordering::byNODES,
               "PA Only supports Ordering::byNODES!");
   Mesh *mesh = fes.GetMesh();
   const FiniteElement &el = *fes.GetFE(0);
   ElementTransformation &T = *mesh->GetElementTransformation(0);
   const IntegrationRule *ir = IntRule ? IntRule : &GetRule(el, T);
   if (DeviceCanUseCeed())
   {
      delete ceedDataPtr;
      ceedDataPtr = new CeedData;
      InitCeedCoeff(Q, *mesh, *ir, ceedDataPtr);
      return CeedMFNLConvectionAssemble(fes, *ir, *ceedDataPtr);
   }
   mfem_error("Not yet implemented.");
}

void VectorConvectionNLFIntegrator::AddMultMF(const Vector &x, Vector &y) const
{
   if (DeviceCanUseCeed())
   {
      return CeedAddMult(ceedDataPtr, x, y);
   }
   else
   {
      MFEM_ABORT("Not yet implemented!");
   }
}

} // namespace mfem
